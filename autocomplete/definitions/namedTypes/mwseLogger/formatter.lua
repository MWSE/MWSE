return {
	type = "value",
	description = [[This is an advanced option and should be used with care.
It allows specifying a custom formatter, allowing for more fine-tuned control over how log messages are printed.
If supplying a formatter, you are responsible for also including the "header" portion of the log.
These can be created by calling the `protected` `makeHeader` method.

Some examples can be found in the `logger/formatters.lua` folder of the core library.
]],
	valuetype = "fun(self: mwseLogger, record: mwseLoggerRecord, ...: string|any|fun(...): ...): string",
}
