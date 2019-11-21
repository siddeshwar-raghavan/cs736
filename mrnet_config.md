## MRNet configuration

### Prepare Source Code

```
wget ftp://ftp.cs.wisc.edu/paradyn/mrnet/mrnet_5.0.1.tar.gz
tar -xzvf mrnet_5.0.1.tar.gz
cd mrnet-3093918/
```

Replace `conf/Makefile.examples.in` with the one in this repo.

Add folder `WordCount/` in this repo to `Examples/`.

### Compile Source Code

```
./configure CC=gcc CXX=g++ CXXFLAGS=-std=c++11 --enable-verbosebuild --with-boost=/usr/include/boost
cd build/$MRNET_ARCH/
make
make examples
sudo make install
sudo make install-examples
```

Hint: Check `INSTALL` for detailed configuration.

### Run Example Applications

Make a temporary folder under project root directory (e.g., `temp/`).

Put the local topology file to `temp/`.

```
cd temp/
WordCount_FE local-1x2.top WordCount_BE ../build/x86_64-pc-linux-gnu/lib/WordCountFilter.so > WordCount.out 2> WordCount.log
```

Check `WordCount.out` to see if the MRNet successfully run or failed.

### Todo

Change the source files under `WordCount` for correct word count logic.
