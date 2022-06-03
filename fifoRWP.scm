(import
  (chibi time)  
  (scheme base)
  (scheme red)  
  (fifoRWP)
  (schemepunk json)  
  (srfi 1)
  (wslib)
  (delay))

(define fifoIn "/tmp/fifoIn")
(define fifoOut "/tmp/fifoOut")

(define (main args) 
 ; (gwinit)
  (let loop ((rv (fifoPoll fifoIn)))  
  (display rv)(newline)

  (if(= rv 0)
    (begin 
      (let ((msg (scheme_read_ws fifoIn)))
      (display msg)      
      (cond 
         ((string=? msg "button1")(scheme_write_ws fifoOut (jsonify "cards" deck)))
         ((string=? msg "button2")(scheme_write_ws fifoOut (jsonify "cards" (knuth-shuffle deck))))
         ((string=? msg "button3")(scheme_write_ws fifoOut (jsonify "clear" deck)))))))
    (delay-seconds 1)         
    (loop (fifoPoll fifoIn))))
    
