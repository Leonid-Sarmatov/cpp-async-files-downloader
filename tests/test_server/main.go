package main

import (
	//"fmt"
	"net/http"
	"log"
)

func main() {
	log.Fatal(http.ListenAndServe(":8080", http.FileServer(http.Dir("./examples_files"))))
}