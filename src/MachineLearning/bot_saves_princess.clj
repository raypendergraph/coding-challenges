(ns bot-saves-princess)

(defn point [x y]
  (double-array [x y]))

(defn discreet-point [[real-x real-y]]
  [(int real-x) (int real-y)])

(defn vector-magnitude [vector]
  (let [square (fn [x] (* x x))]
    (Math/sqrt
      (reduce + (for [x vector] (square x))))))

(defn vector-norm [vector]
  (let [length (vector-magnitude vector)]
    (for [i vector]
      (/ i length))))

(defn vector-subtract [source destination]
  (map - destination source))

(defn vector-add [source destination]
  (map + destination source))

(defn vector-scalar-multiply [vector scalar]
  (map (fn [x] (* scalar x)) vector))

(defn square [x] (* x x))

(defn segment-length [[x-source y-source] [x-dest y-dest]]
   (Math/sqrt (+ (square (- x-dest x-source))
                 (square (- y-dest y-source)))))

(defn calculate-steps [start dest]
  (int (Math/ceil (segment-length start dest))))

(defn next-step [start direction step]
  (discreet-point (vector-add start
                              (vector-scalar-multiply direction step))))

(def dest (point 1 1))
(def start (point 4 9))
;(let [step-count (calculate-steps start dest)
;      direction  (vector-norm (vector-subtract start dest))]
;  (doseq [step (for [n (range 1 step-count)] (next-step start direction n))]
;    (println step )))

(defn create-step-function [source destination]
    (cond
      (> source destination) (fn [current]
                               (if (> current destination) (dec current) current))
      (< source destination) (fn [current]
                               (if (< current destination) (inc current) current))
      :else (fn [current] current)))
(defn moves-remaining? [x y dest-x dest-y]
  (or
    (not (= x dest-x))
    (not (= y dest-y))))

(defn print-step [old-x old-y new-x new-y]
  (println old-x old-y " => " new-x new-y))

(let [[source-x source-y] start
      [dest-x dest-y]     dest
      step-horizontal  (create-step-function source-x dest-x)
      step-vertical    (create-step-function source-y dest-y)]
  (loop [x source-x
         y source-y]
    (when (moves-remaining? x y dest-x dest-y)
        (let [new-x (step-horizontal x)
              new-y (step-vertical y)]
          (do
            (print-step x y new-x new-y)
            (recur  new-x new-y))))))
