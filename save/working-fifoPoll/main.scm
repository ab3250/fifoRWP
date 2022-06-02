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
  (let loop ((rv (fifoPoll "./recv")))
  (display "debug")
  (display rv)(newline)
  (delay-seconds 1)
  (if(= rv 1)(begin 
  (let ((msg (scheme_read_ws)))
  (display msg)
  (scheme_write_ws (jsonify "cards" deck))
      (cond 
         ((string=? msg "button1")(scheme_write_ws (jsonify "cards" deck)))
         ((string=? msg "button2")(scheme_write_ws (jsonify "cards" (knuth-shuffle deck))))
         ((string=? msg "button3")(scheme_write_ws (jsonify "clear" deck)))))))
    (loop (fifoPoll "./recv"))))
    


; (define (main args) 
;   (gwinit)
;   (display "debug 1")
;   (let loop () 
;    (delay-seconds 1)
  ; (display (fifoPoll))
  ; ; (if (>= rv 0)
  ; ;   (begin     
    

  ;  (let ((msg (get_buf)))
  ; ; ;   ;(set_buf "\0")
  ;     (display msg)  ) 
    
    
  
 
  ; (loop)))
  