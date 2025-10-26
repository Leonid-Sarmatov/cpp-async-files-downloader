package main

import (
	//"context"
	//"fmt"
	"log"
	"net/http"
	"os"
	//"os/signal"
	//"syscall"
	"time"
	"path/filepath"
	//"runtime"
)

func AddHeadersMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Disposition", "attachment; filename=i_m_name_form_header_from_test_server")
		next.ServeHTTP(w, r)
	})
}

func main() {
	// Что бы бинарник получил доступ к директрии, нужен абсолютный путь
	exePath, _ := os.Executable()
	exeDir := filepath.Dir(exePath)
	filesDir := filepath.Join(exeDir, "examples_files")
	
	handler := http.FileServer(http.Dir(filesDir))

	//log.Fatal(http.ListenAndServe(":8080", AddHeadersMiddleware(handler)))

	server := &http.Server{
		Addr:         ":8080",
		Handler:      AddHeadersMiddleware(handler),
		ReadTimeout:  15 * time.Second,
		WriteTimeout: 15 * time.Second,
		IdleTimeout:  60 * time.Second,
	}

	log.Printf("Starting server on %s", server.Addr)
	//serverErrors <- server.ListenAndServe()
	log.Fatal(server.ListenAndServe())
	/*
	// Канал для graceful shutdown,
	serverErrors := make(chan error, 1)
	shutdown := make(chan os.Signal, 1)
	signal.Notify(shutdown, os.Interrupt, syscall.SIGTERM)

	// Запускаем сервер в горутине
	go func() {
		log.Printf("Starting server on %s", server.Addr)
		serverErrors <- server.ListenAndServe()
	}()

	// Ожидаем сигнал завершения или ошибку сервера
	select {
	case err := <-serverErrors:
		log.Fatalf("Error starting server: %v", err)

	case <-shutdown:
		log.Println("Starting graceful shutdown...")

		// Даем секунду на завершение активных соединений
		ctx, cancel := context.WithTimeout(context.Background(), 1*time.Second)
		defer cancel()

		if err := server.Shutdown(ctx); err != nil {
			log.Printf("Graceful shutdown failed: %v", err)
			
			if err := server.Close(); err != nil {
				log.Fatalf("Forced shutdown failed: %v", err)
			}
		}

		log.Println("Server stopped gracefully")
	}*/
}