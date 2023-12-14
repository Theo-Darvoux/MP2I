(* Arbre binaire de recherche [ABR], d'après Algorithmique de Cormen *)
type 'a node = Empty | Node of {value : 'a; left : 'a node; right : 'a node}

(** Ajoute l'élément new_val à l'ABR **)
let insert root new_val =
  let rec aux root =
    match root with
    | Empty -> Node {value = new_val; left = Empty; right = Empty}
    | Node {value; left; right} -> 
      if new_val < value then
        Node {value; left = aux left; right}
      else
        Node {value; left; right = aux right}
    in aux root

(** Ajoute chaque élément de values à l'ABR de racine root **)
let rec insert_list root values =
  match values with
  | [] -> root
  | h::t -> insert_list (insert root h) t 

(** Crée un nouvel arbre avec les valeurs de values **)
let new_tree values = 
  match values with
  | [] -> Empty
  | h::t -> insert_list (Node {value = h; left = Empty; right = Empty}) t
(** Renvoie le maximum de l'ABR de racine root **)
let rec maximum root =
  match root with
  | Empty -> failwith "Empty"
  | Node {value; left; right} -> if right = Empty then value else maximum right 
    
(** Supprime l'élément elt de l'ABR**)
let rec delete root elt =
  match root with
  | Empty -> Empty
  | Node {value; left; right} -> 
    if elt > value then Node {value; left; right = delete right elt}
    else if elt < value then Node {value; left = delete left elt; right}
    else 
      match left, right with
      | Empty, Empty -> Empty
      | Empty, _ -> right
      | _, Empty -> left
      | _ -> let y =  maximum left in Node {value = y; left = delete left y; right}

(* Recherche la valeur to_find dans l'ABR de racine root, renvoie le noeud correspondant *)
let rec search root to_find = 
  match root with
  | Empty -> Empty
  | Node {value; left; right} -> 
    if value = to_find then root 
    else if value > to_find then search left to_find
    else search right to_find

(* Renvoie le nombre de noeuds de l'ABR *)
let rec length = function
  | Empty -> 0
  | Node {value; left; right} -> 1 + length left + length right

(* Renvoie la hauteur de l'ABR *)
let rec height = function
  | Empty -> 0
  | Node {value; left; right} -> 1 + max (height left) (height right)

(* Parcourt l'ABR de racine root de manière infixe et applique la fonction f à chaque valeur rencontrée *)
let rec infix root f =
  match root with
  | Empty -> ()
  | Node {value; left; right} -> infix left f; f value; infix right f

(* Évalue la somme des éléments de l'ABR de racine root, dans le cas où ce sont des entiers *)
let rec sum root =
  match root with
  | Empty -> 0
  | Node {value; left; right} -> value + sum left + sum right

(* Affiche chaque valeur de l'ABR par parcours infixe, dans le cas où ce sont des entiers *)
let print_tree root = infix root (fun a -> print_int a; print_char ' '); print_char '\n'