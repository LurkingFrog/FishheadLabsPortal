/** Internal Errors
 *
 * This pattern is used so we can consistently handle all errors using the Result (Either) Monad
 */;

/** An enumeration of all the possible system errors */
type t =
  | /** A catchall for errors that aren't explicitly listed */
    General
  | /** The value is not valid in the given context */
    BadValue
  | /** Tried to update a second value with the same unique identifier */
    DuplicateKey
  | /** Received a None value when Some was expected */
    IsNone
  | /** When an item isn't located in a group */
    NotFound
  | /** Index isn't in the range of the given list/array */
    OutOfRange;
