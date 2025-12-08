# Use GCC 13 official image
FROM docker.io/gcc:13

# Set working directory
WORKDIR /app

COPY ./build_cmake.sh .

RUN . ./build_cmake.sh

RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

RUN rm build_cmake.sh

ENTRYPOINT ["/bin/sh"]
