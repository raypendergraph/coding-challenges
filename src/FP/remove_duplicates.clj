(ns FP.remove-duplicates)

(defn filtered-seq [string]
  (let [found (transient {})
        n (atom 0)]
    (repeatedly (count string )(fn []
                                 (do (swap! n inc )
                                     (if (get found (nth string @n))
                                       nil
                                       (do (conj! found (nth string @n))
                                           (nth string @n))))))))


(println (filtered-seq "aaabcdefabcsdua"))
