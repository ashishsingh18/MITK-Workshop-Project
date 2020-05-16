## ExampleInverter

Contains the "workhorse" code for Workshop example 3. This example module will be a reusable image inverter.

This module defines the functionality. The plugin only presents a user-facing front end and makes a call through the interface of this module.

This has a few benefits:
We can add command line applications or other utilities to the module easily.
We can create other plugins that utilize the module code without changing or breaking our existing plugin. (Reusable functionality!)
The greater separation of concerns helps to keep our code clean.
