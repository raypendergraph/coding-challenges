(ns bot-saves-princess)

(def grid [[\- \- \-] [\- \m \-] [\p \- \-]])

(defn create-step-function [source destination]
  (cond
    (> source destination) (fn [current]
                             (if (> current destination) (dec current) current))
    (< source destination) (fn [current]
                             (if (< current destination) (inc current) current))
    :else (fn [current] current)))

(defn moves-remaining? [row column dest-row dest-column]
  (or
    (not (= column dest-column))
    (not (= row dest-row))))

(defn interleave-all
  [& colls]
  (lazy-seq
    (let [ss (keep seq colls)]
      (when-not (empty? ss)
        (concat (map first ss)
                (apply super-fn (map rest ss)))))))

(defn generate-steps [start-row start-column end-row end-column]
  )

(defn coordinates [row column]
  [row column])

(defn map-row-results [row-results row-number]
  (reduce
    (fn [m [column-number character]]
      (assoc m character (coordinates row-number column-number))) {} row-results))

(defn search-row [row]
  (filter
    (fn [[i c]] (not= c \-))
    (map-indexed vector row)))

(defn find-steps [grid]
  (let [{[m-row m-column] \m [p-row p-column] \p} (into {} (for [row (map-indexed vector grid)]
                                                             (map-row-results (search-row (nth row 1)) (nth row 0))))
        row-step-fn (create-step-function m-row p-row)
        column-step-fn (create-step-function m-column p-column)]
    (loop [row m-row
           column m-column]
      (when (moves-remaining? row column p-row p-column)
        (let [new-row (row-step-fn row)
              new-column (column-step-fn column)]
          (do
            (print-step row column new-row new-column)
            (recur new-row new-column)))))))

(find-steps grid)