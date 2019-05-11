# Kernel

Simple.  Fast.  Modular, no-nonsense game core.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

In order to build the kernel, you will need at least...

- CMake 3.8
- git (obviously)

On Windows, please have:

- Visual Studio 2017

Linux Master Race, you already know:

- gcc 8.0

### Installing

Here are some step by step examples that will get your development environment running.

On Windows:

```
- Install VS2017
- Install Github Desktop
```
Then, on all platforms...

```
- Clone hybridalpaca/gengine-superbuild
- Add hybridalpaca/kernel as a submodule under gengine-superbuild/source/
```

...and edit your `cmake/projects.cmake` to match the new module.  See `hybridalpaca/gengine-superbuild` for more info.

## Running tests

Loaf's kernel currently does not have any tests, as it is a pretty small module.

## Deployment

Once `kernel` is in your superbuild environment, there's not much left to do besides installing some modules and start working!

## Built With

* [CMake](https://cmake.org/) - Our build system

## Contributing

Contact one of the current contributors if you'd like to assist in the dev process.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **stickyfingies** - *Lead Dev* - [stickyfingies](https://github.com/stickyfingies)
* **HenteiLover69** - *Lead Dev* - [henteilover](https://github.com/henteilover69)
* **Kashy K** - *Lead Dev* - [kashy k](https://github.com/kashyk)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Big thanks to [PurpleBooth](https://github.com/purplebooth) for the awesome readme template!
