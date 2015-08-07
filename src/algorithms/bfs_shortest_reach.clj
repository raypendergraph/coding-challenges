(ns algorithms.bfs-shortest-reach
  (:require [clojure.string :as string]))


(defn update-edges! [node graph]
  (let [edges (:edges node)]
    (if (= 0 (count edges))
      (do (println (map str (persistent! (:edges node))))
          (list))
      (do
        (print edges)
        (for [edge-idx (persistent! edges)
              :let [edge-node (nth graph edge-idx)]
              :when (= (:distance edge-node) -1)]
          (update-node! edge-node node))))))


(defn add-edge! [graph node-a-index node-b-index]
  (let [{node-a-edges node-a-index node-b-edges node-b-index} graph]
    (do
      (conj! node-a-edges node-b-index)
      (conj! node-b-edges node-a-index))))

(defn traverse [graph start-node nil-node]
  (loop [queue (list (:id start-node))]
    (if (not (empty? queue))
      (let [[current & rest-of-queue] queue
            new-edges (update-edges! (nth graph current) graph)]
        (do
          (print new-edges)
          (recur (into (or rest-of-queue []) new-edges)))))))

(defn print-graph! [graph]
  (map println
       (map (fn [node] (:distance node)) graph)))

(def T (read-string (read-line)))
(dotimes [t T]
  (let [[N M] (map read-string (string/split (read-line) #"\s+"))
        graph (vec (map () (range (+ N 1))))
        adjacency-matrix (make-array Short/TYPE N N)]
    (do
      (dotimes [m M]
        (let [[node-a-index node-b-index] (map read-string (string/split (read-line) #"\s+"))]
          (add-edge! graph node-a-index node-b-index)))
      (assoc! nil-node :distance 0)
      (let [S (read-string (read-line))]
        (traverse graph (nth graph S) nil-node))
      (print-graph! graph))))
