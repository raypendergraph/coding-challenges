(ns filter-elements)
(use '[clojure.string :only (join split)])

(defn clamp [n lower upper]
  (max lower (min upper n)))

(defn update-frequency [tuple current-idx]
  (if (nil? tuple)
    {:found-at current-idx :count 1}
    {:found-at (get tuple :found-at) :count (inc (get tuple :count))}))

(defn find-freqeuencies [fequency-limit values]
  (let [value-count (count values)]
    (loop [idx 0
           collector (transient {})]
      (if (= idx value-count)
        (persistent! collector)
        (recur (inc idx)
               (assoc! collector
                       (nth values idx)
                       (update-frequency (get collector (nth values idx)) idx)))))))

(defn extract [repetitions numbers]
  (for [sorted-value (sort-by
                       (fn [val] (get-in val [1 :found-at]))
                       (for [[_ value :as kvp] (find-freqeuencies repetitions numbers)
                             :when (>= (get value :count) repetitions)]
                         kvp))]
    (key sorted-value)))

(def T (read-string (read-line)))
(dotimes [n T]
  (let [[N K] (for [num-string (split (read-line) #"\s+")] (read-string num-string))
        list-size (clamp N 1 10000)
        repetitions (clamp K 1 list-size)
        numbers (take N
                      (for [num-string (split (read-line) #"\s+")]
                        (clamp (read-string num-string) 1 (Math/pow 10 9))))
        matches (extract repetitions numbers)]
    (if (empty? matches)
      (println -1)
      (println (join " " matches)))))


;TC2 input
;3
;76 11
;6 10 3 5 8 4 3 3 5 4 4 5 5 6 2 3 9 7 6 4 3 5 3 10 7 2 6 3 9 7 9 7 2 10 3 6 5 2 8 8 3 3 2 6 10 6 6 6 6 9 3 9 3 7 8 9 3 3 4 7 7 5 8 9 4 8 7 3 7 7 2 5 5 5 10 10
;21 1
;8 9 7 6 2 2 3 4 3 9 9 9 9 8 8 4 3 7 2 3 4
;84 9
;3 6 2 10 3 5 5 6 8 5 8 2 3 9 10 3 7 3 5 3 4 7 6 8 10 6 5 7 6 2 6 2 8 4 7 8 10 6 7 6 5 4 4 6 2 5 7 3 6 7 2 8 6 8 4 2 3 7 4 2 3 6 10 9 9 2 7 10 6 8 9 2 4 9 10 8 8 3 6 8 5 7 8 4



;TC2 output
;3
;8 9 7 6 2 3 4
;3 6 2 8 7

