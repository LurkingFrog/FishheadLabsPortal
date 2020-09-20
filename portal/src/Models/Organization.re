/* A group of people coupled with business functions

    This can be anything from a single person up to a multi-national corporation. This is used for defining
    ownership. In financial terms, this is a cost center that can be billed and pay.

    This is designed to be recursive, with a context sensitive overriding of configuration.

    TODO:
   - This should become auto-generated/generated based on JSON served from the process foundry API
   */

type t = {
  guid: string,
  externalId: option(string),
  legalName: option(string),
  parent: option(string),
  children: list(string),
  config: option(string),
};
