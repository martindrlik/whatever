package main

import (
	"crypto/rand"
	"flag"
	"fmt"
	"math/big"
	"os"
	"strconv"
)

var (
	base   = flag.Int("base", 36, "base, for 2 <= base <= 36; uses the lower-case letters 'a' to 'z' for digit values >= 10")
	length = flag.Int("length", 10, "lenght of output word")
	source = flag.String("source", "", "source file for rand")
)

func main() {
	flag.Parse()
	r, err := os.Open(*source)
	if err != nil {
		fmt.Fprintf(os.Stderr, "could not open source file %s: %v\n", *source, err)
		return
	}
	defer r.Close()
	for i := 0; i < *length; i++ {
		n, err := rand.Int(r, big.NewInt(int64(*base)))
		if err != nil {
			fmt.Fprintf(os.Stderr, "rand.Int: %v\n", err)
			break
		}
		fmt.Print(strconv.FormatInt(n.Int64(), *base))
	}
	fmt.Println()
}
