type sessionAction =
  | Create(Session.t)
  | End;

type t =
  | Session(sessionAction);
