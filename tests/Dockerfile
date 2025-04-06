FROM debian:buster

RUN apt-get update && apt-get install -y \
    make \
    gcc \
    bash \
    nano \
    valgrind \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN bash -c "mkdir -p logs_unit"

WORKDIR /app/tests/unit

RUN make run

RUN sleep 2

WORKDIR /app 
