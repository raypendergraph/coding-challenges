(require '[clojure.string :as str])

(defn solve [coll n idx]
  (if (< idx 0)
    (apply println (flatten [n coll]))
    (if (> n (nth coll idx))
      (apply println (flatten [(subvec coll 0 (inc idx)) n (subvec coll (inc idx))]))
      (do
        (apply println (flatten [(subvec coll 0 (inc idx)) (subvec coll idx)]))
        (solve coll n (dec idx))))))

(let [x (read-string (read-line))
      inputs (map read-string (take x (str/split (read-line) #"\s+")))
      [coll last] (split-at (- x 1) inputs)]
  (solve (vec coll) (nth last 0) (- x 2)))



(defn create-point [x y]
  (list x y))

(defn point-x [point]
  (nth point 0))

(defn point-y [point]
   (nth point 1))

(defn point-to-string [point]
  (string "(" (point-x point) ", " (point-y point) ")"))

(defn segment [point-a point-b]
   (list point-a point-b))

(defn segment-length [segment]
  )

(defn segment-start [segment]
  (nth segment 0))

(defn segment-end [segment]
  (nth segment 1))

(defn mid-point [[beginning end]]
  (create-point
    (/ (+ (point-x beginning) (point-x end)) 2 )
    (/ (+ (point-y beginning) (point-y end)) 2 )))