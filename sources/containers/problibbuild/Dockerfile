# Use GCC 13 official image
FROM docker.io/gcc:13

# Set working directory
WORKDIR /app

RUN apt-get update -y && apt-get upgrade -y

RUN apt-get install -y --no-install-recommends \
    cmake

ENTRYPOINT ["/bin/sh"]
