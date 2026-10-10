#!/bin/bash
cmake -B build
cmake --build build 

echo "#!/bin/bash" > run.sh
echo "cd build/src && ./app" >> run.sh
