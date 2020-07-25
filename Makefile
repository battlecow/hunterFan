.PHONY: help

help:
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build-lib: ## Build the library
	g++ -DRPI -c -o hunterFan.o hunterFan.cpp

build-low: ## Build fan-low example
	g++ -DRPI -c -I./ examples/fan-low.cpp -o examples/fan-low.o;\
	g++ -DRPI hunterFan.o examples/fan-low.o -o fan-low -lwiringPi -lwiringPiDev -lcrypt
