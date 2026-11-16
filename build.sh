#!/bin/bash
cmake -B build -DRust_COMPILER=$(rustup which rustc) -DRust_CARGO=$(rustup which cargo	)
cmake --build build 

echo "#!/bin/bash" > run.sh
echo "cd build/src && ./app" >> run.sh
