type formule =
  |C of bool
  |V of int
  |Et of formule * formule
  |Ou of formule * formule
  |Imp of formule * formule
  |Non of formule;;


let f_ex = Et(Imp(V 0, Et(V 1, Ou(Non(V 0),V 2))), Non(Et(V 0, V 2)));;

let rec taille (f:formule) :int = match f with
  |C _ |V _ -> 1
  |Et(f,g) |Ou(f,g) |Imp(f,g) -> 1 + taille f + taille g
  |Non f -> 1 + taille f

let var_min f =
  let rec exists_variable f = match f with
    |C _ -> None
    |V i -> Some i
    |Et(a,b) | Ou(a,b) | Imp(a,b) ->
      let ma = exists_variable a and mb = exists_variable b in
      begin
        match ma, mb with
          |None, None -> None
          |None, Some v | Some v, None -> Some v
          |Some v, Some w -> Some (min v w)
      end
    |Non a -> exists_variable a
  in match (exists_variable f) with
       |None -> failwith "Pas de variable dans cette formule"
       |Some v -> v
;;

let rec elim_constantes formule = match formule with
  |Et(f,g) -> let ef = elim_constantes f and eg = elim_constantes g in
              begin
                match ef, eg with
                |C false, _ | _, C false -> C false
                |C true, h | h, C true -> h
                |a,b -> Et (a,b)
              end
  |Ou(f,g) -> let ef = elim_constantes f and eg = elim_constantes g in
              begin
                match ef, eg with
                |C true, _ | _, C true -> C true
                |C false, h | h, C false -> h
                |a,b -> Ou(a,b)
              end
  |Imp(f,g) -> let ef = elim_constantes f and eg = elim_constantes g in
              begin
                match ef, eg with
                |C false, _ | _, C true -> C true
                |C true, h -> h
                |a, C false -> Non a
                |a,b -> Imp(a,b)
              end
  |Non f -> let ef = elim_constantes f in
            begin
              match ef with
                |C b -> C (not b)
                |a -> Non a
            end
  |f -> f
;;

let rec substitue f i g = match f with
  |V j when i = j -> g
  |Non a -> Non (substitue a i g)
  |Et(a,b) -> Et(substitue a i g, substitue b i g)
  |Ou(a,b) -> Ou(substitue a i g, substitue b i g)
  |Imp(a,b) -> Imp(substitue a i g, substitue b i g)
  | _ -> f
;;

type arbre_decision =
  |Feuille of bool
  |Noeud of int * arbre_decision * arbre_decision
;;

let rec arbre_Quine f =
  match (elim_constantes f) with
  |C b -> Feuille b
  |a -> let i = var_min f in (*qui existe par correction de elim_constantes et le cas de base*)
        let f_bot = substitue f i (C false) and
            f_top = substitue f i (C true)
        in Noeud(i, arbre_Quine f_bot, arbre_Quine f_top)
;;

let satisfiable f =
  let rec exists_feuille_top a = match a with
    |Feuille b -> b
    |Noeud(_,g,d) -> exists_feuille_top g || exists_feuille_top d
  in exists_feuille_top (arbre_Quine f)
;;






type graphe = int list array;;

let variable i c k = V (i*k + c);;

let rec binarise_ou formules = match formules with
  |[] -> C false
  |f::rf -> Ou(f, binarise_ou rf)
;;

let rec binarise_et formules = match formules with
  |[] -> C true
  |f::rf -> Et(f, binarise_et rf)
;;

let liste_couleurs k = List.init k (fun i -> i);;

let est_colorie i k =
  binarise_ou (List.init k (fun c -> variable i c k))
;;

let contraintes g i k =
  let contraintes_couleur g i c k =
    let voisins = List.map (fun j -> variable j c k) g.(i) in (*calcul des voisins du sommet i*)
    let  autres_couleurs = List.filter (fun x -> x <> c) (liste_couleurs k) in (*donne les couleurs différentes de c*)
    let var_contradictoires = List.map (fun c' -> variable i c' k) autres_couleurs (*donne toutes les variables v_i,c' sauf celle pour c' = c*)
    in Imp(variable i c k, Non (binarise_ou (voisins @ var_contradictoires)))
  in binarise_et (List.init k (fun c -> contraintes_couleur g i c k))
;;

let encode g k =
  let nb_sommets = Array.length g in
  binarise_et (List.init nb_sommets (fun i -> Et(est_colorie i k, contraintes g i k)))
;;

let petersen =
  [|
    [4; 5; 6];
    [6; 7; 8];
    [5; 8; 9];
    [4; 7; 9];
    [0; 3; 8];
    [0; 2; 7];
    [0; 1; 9];
    [1; 3; 5];
    [1; 2; 4];
    [2; 3; 6]
  |];;

let est_k_coloriable g k =
  satisfiable (encode g k)
;;

est_k_coloriable petersen 3;;

let nombre_chromatique g =
  let nb = ref 1 in
  while not (est_k_coloriable g !nb) do
    nb := !nb +1
  done;
  !nb
;;

nombre_chromatique petersen;;

let graphe_mystere =
  [|[1;3;6;8];
    [0;2;5;7];
    [1;4;6;9];
    [0;4;5;9];
    [2;3;7;8];
    [1;3;10];
    [0;2;10];
    [1;4;10];
    [0;4;10];
    [2;3;10];
    [5;6;7;8;9]|]
;;

nombre_chromatique graphe_mystere;;




(*Amélioration par simulation d'une évaluation paresseuse*)
let rec simplifie x b = function
  | V i when i = x -> C b
  | Et (f, g) ->
     begin match simplifie x b f with
     | C false -> C false
     | C true -> simplifie x b g
     | f' -> begin match simplifie x b g with
             | C false -> C false
             | C true -> f'
             | g' -> Et (f', g')
             end
     end
  | Ou (f, g) ->
     begin match simplifie x b f with
     | C true -> C true
     | C false -> simplifie x b g
     | f' -> begin match simplifie x b g with
             | C true -> C true
             | C false -> f'
             | g' -> Ou (f', g')
             end
     end
  | Imp (f, g) ->
     begin match simplifie x b f with
     | C false -> C true
     | C true -> simplifie x b g
     | f' -> begin match simplifie x b g with
             | C true -> C true
             | C false -> Non f'
             | g' -> Imp (f', g')
             end
     end
  | Non f ->
     begin match simplifie x b f with
     | Non f' -> f'
     | C b' -> C (not b')
     | f' -> Non f'
     end
  | f -> f

let rec arbre_Quine2 f =
  match f with
  |C b -> Feuille b
  |a -> let i = var_min f in
        let f_bot = simplifie i false f  and
            f_top = simplifie i true f
        in Noeud(i, arbre_Quine f_bot, arbre_Quine f_top)
;;

let satisfiable2 f =
  let rec exists_feuille_top a = match a with
    |Feuille b -> b
    |Noeud(_,g,d) -> exists_feuille_top g || exists_feuille_top d
  in exists_feuille_top (arbre_Quine2 f)
;;

let nombre_chromatique2 g =
  let nb = ref 1 in
  while not (satisfiable2 (encode g !nb)) do
    nb := !nb +1
  done;
  !nb
;;

nombre_chromatique2 graphe_mystere;;


open Unix;;
open Printf;;
open Random;;
Random.self_init();;

let temps1 (f:'a->'b) (x:'a) :'b  =
    let t = Unix.gettimeofday () in
    let fx = f x in
    Printf.printf "temps d'execution : %f secondes \n"
        (Unix.gettimeofday () -. t);
    fx
;;

(*On gagne 3 secondes avec la version 2, yay !*)


