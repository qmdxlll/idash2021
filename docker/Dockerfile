#基于的基础镜像
FROM ubuntu:18.04

#ENV  PYTHON_VERSION=3.8.2

RUN apt-get update && \apt-get install  -y build-essential software-properties-common python3.8 git cmake cmake-curses-gui vim
#代码添加到code文件夹
ADD ./app /code

RUN git clone --recurse-submodules --branch=master git://github.com/tfhe/tfhe.git && \
    cd tfhe  && \
mkdir build && \
cd build && \
cmake ../src &&\
make&&make install &&\
echo TFHE_PREFIX=/usr/local > ~/.bashrc &&\
echo 'export C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include' >> ~/.bashrc  &&\
echo 'export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include' >> ~/.bashrc &&\
echo 'export LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib' >> ~/.bashrc &&\
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib' >> ~/.bashrc &&\
exec bash && source ~/.bashrc
 
# 设置code文件夹是工作目录
WORKDIR /code
