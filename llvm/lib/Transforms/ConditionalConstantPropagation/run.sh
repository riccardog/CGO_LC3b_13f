input=$1
basename=`basename $1 | awk -F'.' '{print $1}'`
../../../Debug+Asserts/bin/opt -load ../../../Debug+Asserts/lib/ConditionalConstantPropagation.so -conditional-constant-propagation < $input -f > ${basename}_transformed.bc
