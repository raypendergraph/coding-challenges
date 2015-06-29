(ns rotate-string)

(defn index-rotation-fn [length rotation]
  (fn [idx]
    (mod (+ rotation idx) length)))


(defn rotate-string [string string-length rotation]
  (for [idx (range string-length)
        :let [rotation-of (index-rotation-fn string-length rotation)]]
    (.charAt string (rotation-of idx))))


(defn print-rotations! [string]
  (let [string-length (count string)]
    (doseq [rotation (range 1 (+ string-length 1))]
      (doseq [shifted-char (rotate-string string string-length rotation)]
        (print shifted-char))
      (print " "))))


(let [test-cases (read-string (read-line))]
  (dotimes [_ test-cases]
    (do
      (print-rotations! (read-line))
      (print "\n"))))





