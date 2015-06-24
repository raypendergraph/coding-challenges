(ns counting-sort)
(require '[clojure.string :as string])

(defn read-values
  [count]
  (map read-string
       (take count
             (string/split (read-line) #"\s+"))))

(defn faster-frequencies
  [data range-size]
  (let [counts (int-array range-size 0)]
    (do
      (doseq [number data]
        (aset counts number (inc (aget counts number))))
      counts)))


(defn number-frequencies [numbers min max]
  (for [i (range min (inc max))
        :let [freqs (faster-frequencies numbers (inc max))]]
    (get freqs i 0)))


(let [n (read-string (read-line))]
  (apply print (faster-frequencies (read-values n) 100)))

