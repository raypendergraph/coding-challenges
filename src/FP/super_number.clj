(ns FP.super-number)

(def numbers {\1 1, \2 2, \3 3, \4 4, \5 5, \6 6, \7 7, \8 8, \9 9, \0 0})
(def mem-str (memoize str))
(declare mem-digit-str)

(defn digit-string
  [digits]
  (let [digit-count (count digits)]
    (if (= digit-count 1)
      (get numbers (first digits))
      (let [split (unchecked-divide-int digit-count 2)]
        (+
          (mem-digit-str (take split digits))
          (mem-digit-str (drop split digits)))))))

(def mem-digit-str (memoize digit-string))

(defn do-main
  []
  (let [[n k] (take 2 (clojure.string/split (read-line) #"\s+"))
        first (* (mem-digit-str k) (read-string n))]
    (loop [current first]
      (if (< current 10)
        current
        (recur (mem-digit-str (mem-str current)))))))

(println (do-main))