(ns bot-saves-princess)

(def dest (point 1 1))
(def start (point 4 9))

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
