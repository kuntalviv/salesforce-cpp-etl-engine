const addon = require('./build/Release/process_data');

console.log(addon.processData([{ "id": 1, "PAN": "123" }])); // Output: Hello from C++!
