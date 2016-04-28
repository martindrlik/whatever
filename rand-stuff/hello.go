package main

import (
	"html/template"
	"net/http"
)

func init() {
	http.HandleFunc("/", root)
}

var tmpl = template.Must(template.ParseFiles("index.html"))

func root(w http.ResponseWriter, r *http.Request) {
	err := tmpl.Execute(w, nil)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}
