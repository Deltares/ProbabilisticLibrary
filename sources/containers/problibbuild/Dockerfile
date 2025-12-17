# Use GCC 13 official image
FROM docker.io/gcc:13

# Set working directory
WORKDIR /app

COPY ./build_cmake.sh .

RUN . ./build_cmake.sh \
&& ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake \
&& ln -s /opt/cmake/bin/ctest /usr/local/bin/ctest \
&& rm build_cmake.sh

ENTRYPOINT ["/bin/sh"]
