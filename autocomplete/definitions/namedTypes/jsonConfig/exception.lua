return {
	type = "value",
	description = [[When exception is raised, this callback function will be called whenever the encoder cannot encode a given value. The parameters are `reason`, `value`, `state` and `defaultmessage`. `reason` is either `"reference cycle"`, `"custom encoder failed"` or `"unsupported type"`. `value` is the original value that caused the exception, `state` is this `jsonConfig` table, `defaultmessage` is the message of the error that would usually be raised. The callback can either return `true` and write encoded `value` directly to the `buffer` or you can return the encoded string. If the callback decides to raise an error, return `nil` and the desired error message. An example implementation for an exception function is given in `json.encodeexception`.]],
	valuetype = [[(fun(reason: "reference cycle"|"custom encoder failed"|"unsupported type", value, state: jsonConfig, defaultMessage: string): string|true|nil, string?)?]],
}
