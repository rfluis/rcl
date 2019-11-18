#!/bin/bash
for i in $(ls | grep "\.hpp") ; do echo \#include \"$i\" ; done | grep -v rcl.hpp | grep -v rcl-all.hpp > rcl-all.hpp
