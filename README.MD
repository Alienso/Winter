# Winter

Spring clone in C++

## Features

- Http Server that can handle HTTP requests
- Routing using @Get, @Post, ... annotations
- Json serialization from http request body
- Entity serialization from postgres database and CRUD operations
- Logging library
- Reflection
- @Autowired


### Dependencies:
* https://github.com/catchorg/Catch2/tree/v2.x - V2.X
* https://github.com/chriskohlhoff/asio

## How to Run:

With cmake, first build and run pre_processor, so it can generate required files. After that you can just run the app itself.

Note: Pre processor run is required before each application build because of the way reflection and annotations are set up.

### TODO:
* sql object bindings
* Object primitive types
* yml config file