(ns FP.area-and-volume)
; Definite integrals
;https://www.hackerrank.com/challenges/area-under-curves-and-volume-of-revolving-a-curv

(require '[clojure.string :as str])

(defn kernel-fn [a x b]
  (* a (Math/pow x b)))

(defn test-interval [a x b]
  (do
    (println a x b " = " (kernel-fn a x b))
    (kernel-fn a x b)))

(defn build-volume-fn [a-values b-values kernel-fn]
  (fn [x]
    (let [kernel (fn [a b] (kernel-fn a x b))]
      (* Math/PI
         (Math/pow (apply + (map kernel a-values b-values))
                   2)))))

(defn build-area-fn [a-values b-values kernel-fn]
  (fn [x]
    (let [kernel (fn [a b] (kernel-fn a x b))]
      (apply + (map kernel a-values b-values)))))

(defn fx-over-interval [f-of-x interval-start interval-stop delta-x]
  (let [steps-per-interval (/ 1 delta-x)
        total-steps        (* steps-per-interval (- interval-stop interval-start))]
    (apply + (for [current-step (range 1 total-steps)]
               (let [x (+ interval-start (if (= current-step 0) 0 (/ current-step steps-per-interval)))]
                 (* delta-x (f-of-x x)))))))

(defn solve [interval-start interval-stop fn]
  (fx-over-interval fn interval-start interval-stop 0.001))

;(println (format "%.3f" (solve 1 4 (build-area-fn [1 2 3 4 5] [6 7 8 9 10] kernel-fn))))
;(println (format "%.3f" (solve 1 4 (build-volume-fn [1 2 3 4 5] [6 7 8 9 10] kernel-fn))))
(let [a-values (map read-string (str/split (read-line) #" "))
      b-values (map read-string (str/split (read-line) #" "))
      [interval-start interval-stop] (map read-string (str/split (read-line) #" "))]
  (do
    (println (format "%.1f" (solve interval-start interval-stop (build-area-fn a-values b-values kernel-fn))))
    (println (format "%.1f" (solve interval-start interval-stop (build-volume-fn a-values b-values kernel-fn))))))
