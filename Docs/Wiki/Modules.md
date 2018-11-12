# Modules

From just looking at the code, loading a module and registering it with the kernel appears extremely confusing.  The basic process goes like this:

- Kernel creates a `SystemServices` object and fills it with module->system interfaces
- Kernel loads module dll and passes it the system services
- Module fills out a `ModuleDesc` and sends it back to the kernel throuth the system services
- Kernel validates module, ensuring it is version compatible
