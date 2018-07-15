FROM gcc
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN ./build.bat
RUN mkdir data
CMD ["./da2"]