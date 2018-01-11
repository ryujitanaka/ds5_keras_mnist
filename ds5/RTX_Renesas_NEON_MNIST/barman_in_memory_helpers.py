#
# Copyright (C) Arm Limited 2017. All rights reserved.
#
# Use, modification and redistribution of this file is subject to your possession of a valid
# DS-5 end user licence agreement and your compliance with all applicable terms and
# conditions of such licence agreement
#

from arm_ds.debugger_v1 import Debugger
from arm_ds.debugger_v1 import DebugException
from com.arm.debug.dtsl import ConnectionManager
import os.path
import re
import sys

BM_PROTOCOL_HEADER_ = "bm_protocol_header_"

def _find_bm_protocol_header_variable(currentContext):
    fileMatcher = re.compile("^Global and static variables in \"([^\\\"]+)\":$")
    infoVariablesOutput = currentContext.executeDSCommand("info variables")
    variableToFindWithSpaces = " %s" % (BM_PROTOCOL_HEADER_,)
    currentFile = None

    for line in re.split("[\\n\\r]+", infoVariablesOutput):
        fileMatch = fileMatcher.match(line)
        if (fileMatch is not None):
            currentFile = fileMatch.group(1)
        elif (currentFile is not None) and (line.find(variableToFindWithSpaces) > 0):
            return "\"%s\"::%s" % (currentFile, BM_PROTOCOL_HEADER_)

    return None

def _get_barman_memory_buffer_offset_and_size(currentContext):

    print "\nFinding barman variable\n"
    bm_protocol_header_variable = _find_bm_protocol_header_variable(currentContext)
    if (bm_protocol_header_variable is None):
        print "\nNot found\n"
        return None

    print "\nFound barman protocol data variable %s\n" % (bm_protocol_header_variable,)

    variableService = currentContext.getVariableService()

    try:
        bph = variableService.readValue(bm_protocol_header_variable)
        if (len(bph) > 8):
            print "\n%s appears to be a struct. Are you sure you have configured barman for in memory capture?\n"
            return None

        bph = long(bph)
        bph_header_length = long(variableService.readValue("%s->header_length" % (bm_protocol_header_variable,)))
        bph_base_ptr = long(variableService.readValue("%s->data_store_parameters.base_pointer" % (bm_protocol_header_variable,)))
        bph_buffer_length = long(variableService.readValue("%s->data_store_parameters.buffer_length" % (bm_protocol_header_variable,)))
        bph_total_written = long(variableService.readValue("%s->data_store_parameters.total_written" % (bm_protocol_header_variable,)))
        offset = (bph_base_ptr - bph)
        length = (bph_buffer_length if bph_buffer_length < bph_total_written else bph_total_written)

        if (bph_header_length < 0) or (bph_buffer_length < 0) or (bph_total_written < 0) or (offset < bph_header_length):
            print "\nInvalid header contents\n"
            return None

        return {'base'          : bph,
                'data_length'   : length + offset,
                'buffer_length' : bph_buffer_length,
                'total_written' : bph_total_written}
    except DebugException, e:
        print "Debug exception %s occurred - could not read barman header" % (e.getMessage(), )
        return None

"""
USECASE

$Title$ Get Parameters
$Description$ Print out the barman agent in-memory capture buffer properties
$Run$ get_parameters
"""
def get_parameters(options):
    debugger = Debugger()
    debugger.stop()
    currentContext = debugger.getCurrentExecutionContext()

    parameters = _get_barman_memory_buffer_offset_and_size(currentContext)
    if (parameters is not None):
        base = parameters['base']
        data_length = parameters['data_length']
        buffer_length = parameters['buffer_length']
        total_written = parameters['total_written']

        print "Barman memory buffer details:"
        print "    Base address:  0x%016x" % (base,)
        print "    Dump length:   %d" % (data_length,)
        print "    Bytes written: %d of %d (%0.1f%%)" % (total_written, buffer_length, (100.0 * total_written / buffer_length))
        if (total_written > buffer_length):
            print "       (Values greater than 100% are allowable for circular buffers and indicate the number of times the buffer has been reused)"
        print
        print "To dump this buffer use the command:"
        print "    dump memory <PATH> 0x%x +%d" % (base, data_length)
        print "Or use the usecase command 'dump':"
        print "    usecase run \"%s\" dump --file <PATH>" % (sys.argv[0],)

def _dump_options():
    return [UseCaseScript.stringOption(name="file", displayName="File to dump to", defaultValue="")]

def _dump_validate(file):
    if (file is None) or (len(file) == 0):
        print "ERROR: --file '%s' is invalid" % (file,)
        return False
    else:
        file = os.path.abspath(file)
        if (os.path.isfile(file)):
            print "ERROR: --file '%s' already exists" % (file,)
            return False
        elif (os.path.exists(file)):
            print "ERROR: --file '%s' does not point to a file" % (file,)
            return False
        else:
            dir = os.path.dirname(file)
            if (not os.path.isdir(dir)):
                print "ERROR: Directory '%s' does not exist" % (dir,)
                return False
    return True

"""
USECASE

$Title$ Dump buffer
$Description$ Dump the barman agent in-memory capture buffer to a file
$Run$ dump
$Options$ _dump_options
"""
def dump(options):
    file = os.path.abspath(options.getOptionValue("file"))
    if _dump_validate(file):
        debugger = Debugger()
        debugger.stop()
        currentContext = debugger.getCurrentExecutionContext()

        parameters = _get_barman_memory_buffer_offset_and_size(currentContext)
        if (parameters is not None):
            base = parameters['base']
            data_length = parameters['data_length']
            commandString = "dump binary memory \"%s\" 0x%x +%d" % (file, base, data_length,)

            print "Executing command:"
            print "    %s" % (commandString, )
            print
            currentContext.executeDSCommand(commandString)
