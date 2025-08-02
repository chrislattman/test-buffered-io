java:
	java TestIO.java

python:
	python3 testio.py

go:
	go run go/testio.go

rust:
	cargo run -q

cpp:
	g++ -o testio testio.cpp && ./testio

csharp:
	dotnet run

clean:
	rm -rf *.class target testio output.bin bin obj

.PHONY: java python go rust cpp csharp
