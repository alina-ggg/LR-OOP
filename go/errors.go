package ads

import "errors"

var (
	ErrEmpty           = errors.New("ads: structure is empty")
	ErrIndexOutOfRange = errors.New("ads: index out of range")
	ErrKeyNotFound     = errors.New("ads: key not found")
	ErrBadFormat       = errors.New("ads: bad serialization format")
)
