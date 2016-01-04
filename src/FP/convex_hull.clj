(ns FP.convex-hull)



(defn compare-points
  [[x1 y1]
   [x2 y2]]
  (let [x (- x1 x2)]
    (if (= x 0)
      (- y1 y2)
      x)))



(defn cross
  [[origin-x origin-y]
   [a-x a-y]
   [b-x b-y]]
  (-
    (* (- a-x origin-x) (- b-y origin-y))
    (* (- a-y origin-y) (- b-x origin-x))))


(def input [[5 6] [9 2] [2 7]])

(defn between?
  [])

(defn test-point
  [[x y]
   [x-min-x x-min-y :as x-min]
   [x-max-x x-max-y :as x-max]
   [y-min-x y-min-y]
   [y-max-x y-max-y]]
  ([]))

(defn test-extremities
  [points]                                       ;0       1       2       3       4       5       6       7
  (let [extremities (make-array Integer/TYPE 8)] ;x-min-x x-min-y y-min-x y-min-y x-max-x x-max-y y-max-x y-max-y
    (doseq [point points]
      )))


