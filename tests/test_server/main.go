package main

import (
	//"fmt"
	"net/http"
	"log"
)

func AddHeadersMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Disposition", "attachment; filename=i_m_name_form_header_from_test_server")
		next.ServeHTTP(w, r)
	})
}

func main() {
	fileServer := http.FileServer(http.Dir("./examples_files"))
	log.Fatal(http.ListenAndServe(":8080", AddHeadersMiddleware(fileServer)))
}