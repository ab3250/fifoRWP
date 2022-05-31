(define-library (gwsocket)
  (import
    (scheme base))
(export
	gwinit
	read_from_page
	send_to_page
	get_buf
	set_buf)
	(include-shared "gwsocket"))

