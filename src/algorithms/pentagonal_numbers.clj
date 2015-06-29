(ns algorithms.pentagonal-numbers)


(defn pentagonal [n]
  (/
    (-
      (* 3
         (* n n))
      n)
    2))


(let [T (read-string (read-line))]
  (dotimes [_ T]
    (println (pentagonal (read-string (read-line))))))