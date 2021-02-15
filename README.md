[![Build Status](https://travis-ci.org/maximilianvoss/acll.svg?branch=master)](https://travis-ci.org/maximilianvoss/acll)

# Another C LinkedList Library

This library is a small helper when it comes to linked lists. It supports filtering for the next/previous/first/last
element and for each function is a unit test available.

## Compiling the latest code

```bash
git clone https://github.com/maximilianvoss/acll.git
cd acll
cmake -G "Unix Makefiles"
make 
make test  
sudo make install
```

## Build Dependencies

```bash
(git clone https://github.com/maximilianvoss/casserts.git && cd casserts && cmake -G "Unix Makefiles" && make && sudo make install)
```
