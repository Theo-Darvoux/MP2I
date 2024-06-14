open Images
open Yeux_rouges

let dimensions (img : image) = Array.length img.pix.(0), Array.length img.pix
let voisins (img : image) (x, y : int * int) =
  let mx, my = dimensions img in
  let res = ref [] in
  if x + 1 < mx && rouge img.pix.(y).(x+1) then res := (x+1, y)::!res;
  if x - 1 >= 0 && rouge img.pix.(y).(x-1) then res := (x-1, y)::!res;
  if y + 1 < my && rouge img.pix.(y+1).(x) then res := (x, y+1)::!res;
  if y - 1 >= 0 && rouge img.pix.(y-1).(x) then res := (x, y-1)::!res;
  !res

let rec ajoute_sommets q l =
  match l with
  | [] -> ()
  | (x, y)::tl -> Queue.add (x, y) q; ajoute_sommets q tl

(* Dijkstra *)
let rec composante_connexe (img : image) (x0, y0 : int * int) =
  let decouverts = Queue.create () in ajoute_sommets decouverts [(x0, y0)];
  let mx, my = dimensions img in
  let res = Array.init (mx * my) (fun _ -> false) in
  while not (Queue.is_empty decouverts) do
    let x, y = Queue.take decouverts in
    if res.(y * mx + x) = true then () else begin
      res.(y * mx + x) <- true;
      ajoute_sommets decouverts (voisins img (x, y))
    end
  done;
  res

let modif_coul (img : image) (x, y : int * int) = 
  let r, g, b = img.pix.(y).(x).(0), img.pix.(y).(x).(1), img.pix.(y).(x).(2) in
  img.pix.(y).(x) <- [|(g + b) / 2; g; b|]

let enlever_yeux_rouges (img : image) (arr : bool array) =
  let mx, my = dimensions img in
  for y = 0 to my - 1 do
    for x = 0 to mx - 1 do
      if arr.(y * mx + x) then modif_coul img (x, y)
    done
  done

let main file (x, y) (x', y') =
  let img = charger_image file in
  enlever_yeux_rouges img (composante_connexe img (x, y));
  enlever_yeux_rouges img (composante_connexe img (x', y'));
  sauvegarder_image img (String.sub file 0 (String.length file - 4) ^ "rendu.ppm");;

(* C'est interdit les doubles poitns virgule !!!!*)

main "photo1.ppm" (427, 672) (1548, 648);; 
main "photo2.ppm" (220, 393) (369, 379);;