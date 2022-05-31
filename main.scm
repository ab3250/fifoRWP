(import
  (chibi time)  
  (scheme base)
  (scheme red)  
  (gwsocket)
  (schemepunk json)  
  (srfi 1)
  (wslib)
  (delay))


(define (main args) 
  (gwinit)
  (let loop ()
    (read_from_page)
    (let ((msg (get_buf)))
    (display msg)   
    (cond 
        ((string=? msg "button1")(send_to_page (jsonify "cards" deck)))
        ((string=? msg "button2")(send_to_page (jsonify "cards" (knuth-shuffle deck))))
        ((string=? msg "button3")(send_to_page (jsonify "clear" deck)))))
  (loop)))