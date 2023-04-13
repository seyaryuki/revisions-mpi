taquin: vector.cmo heap.cmo taquin.cmo
	ocamlc -o taquin vector.cmo heap.cmo taquin.cmo

vector.cmo: vector.ml
	ocamlc -c vector.mli vector.ml

heap.cmo: heap.ml
	ocamlc -c heap.mli heap.ml

taquin.cmo: taquin.ml
	ocamlc -c taquin.ml

clean:
	rm *.cmi *.cmo taquin

