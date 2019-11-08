```
 _   _  ___  ____  _____      _    ____  ____   ___  _   _ ____
| \ | |/ _ \|  _ \| ____|    / \  |  _ \|  _ \ / _ \| \ | / ___|
|  \| | | | | | | |  _|     / _ \ | | | | | | | | | |  \| \___ \
| |\  | |_| | |_| | |___   / ___ \| |_| | |_| | |_| | |\  |___) |
|_| \_|\___/|____/|_____| /_/   \_\____/|____/ \___/|_| \_|____/
```

# Node Addon Example

This repo is a fast demonstration of how to use C++ as a native node module.
This is useful for increasing performance, making use of pre-built libraries, etc.

This demo makes use of a weight template class to create a new weight library for Node.

Additionally, it uses CMake.js,

# Analysis

Even simple operations are more efficient when performed on c++. A comparison of a simple add function that takes two arguments, checks if they are numbers, then adds them has c++ running almost double the speed of JS.

# Building

Run either of the following:

`yarn` or `npm install`

Then run the following to compile the c++ to your machine (make sure you have CMake installed).

`yarn build` or `npm run build`

Finally, run the following to run the node program that calls the c++ module.

`yarn start` or `npm run start`
