# Use GCC 13 official image
FROM docker.io/gcc:13

# Set working directory
WORKDIR /app

RUN apt-get update -y && apt-get upgrade -y

RUN apt-get install -y --no-install-recommends \
    cmake \
    python3-dev \
    python3-h5py \
    python3-pip \
    python3-venv

# Upgrade Pip
#RUN python3 -m pip install --upgrade pip

# Install Python packages
RUN apt-get install -y \
    python3-numpy \
    python3-matplotlib \
    python3-pytest \
    python3-notebook

ENTRYPOINT ["/bin/sh"]
