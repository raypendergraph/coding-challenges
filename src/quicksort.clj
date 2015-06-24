(ns quicksort)
(require '[clojure.string :as string])

(defn read-values [count]
  (map read-string
       (take count
             (string/split (read-line) #"\s+"))))

(defn quicksort-partition
  "A quick-sort partition function that utilizes juxt.
  Takes values, uses the first number as the pivot and uses juxt[apose] to partition the sequence around it.

  Inputs:
    values - A sequence of values
  Output:
    A partition => ([lower numbers] [pivot + higher numbers])"

  [[pivot :as values]]                                      ;1
  (let [split-fn (juxt filter remove)                       ;2
        predicate-fn (partial > pivot)]                     ;3
    (split-fn predicate-fn values)))                        ;4

(defn do-sort! [values]
  "Quicksort function with side effects. Prints each partition after recombination.

  Inputs
     values - a sequence of (presumably) unsorted values.
  Outputs
     The sorted sequence."
  (if (<= (count values) 1)
    values
    (let [[lower-values [pivot & upper-values]] (quicksort-partition values)
          sorted-values (flatten [(do-sort! lower-values)
                                  pivot
                                  (do-sort! (or upper-values []))])]
      (do
        (apply println sorted-values)
        sorted-values))))

(let [n      (read-string (read-line))
      values (read-values n)]
  (do-sort! values))




;1  This is called "destructuring" basically taking [0] putting
;   it in pivot and the whole thing in values.

;2  A custom split function created by composing a juxtaposition
;   of the filter then remove functions.

;3  A predicate created by partially applying the > fn to the known
;   pivot. The rhs will stream in from the array.

;4  Our split-fn returns two sequences, those where predicate-fn
;   was true and those where it was false.