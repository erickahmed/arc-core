FROM occt-builder:latest AS ffi-base

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y curl build-essential

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

WORKDIR /app
COPY . .

ENV LD_LIBRARY_PATH=/usr/local/lib:/app/build
ENV RUSTFLAGS="-L /usr/local/lib -L /app/build"

RUN cargo build

FROM ubuntu:24.04 AS ffi-unit-testing

RUN apt-get update && apt-get install -y \
    build-essential cmake git curl \
    libfreetype-dev \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

COPY --from=occt-builder:latest /usr/local /usr/local
COPY --from=occt-builder:latest /app/build /app/occt-build
COPY --from=ffi-base /app /app

WORKDIR /app

ENV LD_LIBRARY_PATH=/usr/local/lib:/app/occt-build
ENV RUSTFLAGS="-L /usr/local/lib -L /app/occt-build"

CMD ["cargo", "test", "--verbose", "--", "--nocapture"]
