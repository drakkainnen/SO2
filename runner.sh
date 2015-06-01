#!/bin/bash

find . -regex .+unittest$ -type f -exec {} \;
