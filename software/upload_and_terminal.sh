#!/bin/bash

nios2-download -g *.elf && sleep 1 && nios2-terminal --instance=0
