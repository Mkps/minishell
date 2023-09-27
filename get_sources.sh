#!/bin/bash

find source/* -name "*.c" | cut -d'/' -f2- | sed s/$/' \\'/
