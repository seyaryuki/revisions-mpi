(***** La taille des grilles est définie globalement *****)
let n = 4


(***** Définition des types *****)
type state = {
  grid : int array array;
  mutable i : int;
  mutable j : int;
  mutable h : int;
}

type direction = U | D | L | R | No_move


(***** Fonctions fournies, à décrire avant d'utiliser *****)
let print_state state =
  for i = 0 to n - 1 do
    for j = 0 to n - 1 do
      if i = state.i && j = state.j then print_string "   "
      else Printf.printf "%2d " state.grid.(i).(j)
    done;
    print_newline ()
  done

let delta = function
  | U -> (-1, 0)
  | D -> (1, 0)
  | L -> (0, -1)
  | R -> (0, 1)
  | No_move -> assert false

let distance i j value =
  let i_target = value / n in
  let j_target = value mod n in
  abs (i - i_target) + abs (j - j_target)















(***** Exemples pour les tests *****)

(* La configuration finale *)
let final =
  let m = Array.make_matrix n n 0 in
  for i = 0 to n - 1 do
    for j = 0 to n - 1 do
      m.(i).(j) <- i * n + j
    done
  done;
  {grid = m; i = n - 1; j = n - 1; h = 0}

(***** A DECOMMENTER QUAND LES FONCTIONS DE LA PARTIE 1 SERONT ECRITES

(* Génère une configuration en appliquant une série de mouvements à la configuration finale *)
(* La configuration s généère vérifie évidemment d(s,final) <= nb_moves *)
let random_state nb_moves =
  let state = copy final in
  for i = 0 to nb_moves - 1 do
    let moves = possible_moves state in
    let n = List.length moves in
    apply state (List.nth moves (Random.int n))
  done;
  state

(* distance 10 *)
let ten =
  let moves = [U; U; L; L; U; R; D; D; L; L] in
  let state = copy final in
  List.iter (apply state) moves;
  state

(* distance 20 *)
let twenty =
  {grid =
    [| [|0; 1; 2; 3|];
      [|12; 4; 5; 6|];
      [|8; 4; 10; 11|];
      [|13; 14; 7; 9|] |];
   i = 1; j = 1; h = 14}

(* distance 30 *)
let thirty =
  {grid =
     [| [|8; 0; 3; 1|];
       [|8; 5; 2; 13|];
       [|6; 4; 11; 7|];
       [|12; 10; 9; 14|] |];
   i = 0; j = 0; h = 22}

(* distance 40 *)
let forty =
  {grid =
     [| [|7; 6; 0; 10|];
       [|1; 12; 11; 3|];
       [|8; 4; 2; 5|];
       [|8; 9; 13; 14|] |];
   i = 2; j = 0; h = 30}

(* distance 50 *)
let fifty =
  let s =
    {grid =
       [| [| 2; 3; 1; 6 |];
          [| 14; 5; 8; 4 |];
          [| 15; 12; 7; 9 |];
          [| 10; 13; 11; 0|] |];
     i = 2;
     j = 3;
     h = 0} in
  compute_h s;
  s

(* distance 64 *)
let sixty_four =
  let s =
    {grid =
       [| [| 15; 14; 11; 7|];
          [| 5; 9; 12; 4|];
          [| 3; 10; 13; 8|];
          [| 2; 6; 0; 1|] |];
     i = 0;
     j = 0;
     h = 0} in
  compute_h s;
  s

FIN DES TESTS *****)


(***** Le "main" *****)

let _ = print_state final
