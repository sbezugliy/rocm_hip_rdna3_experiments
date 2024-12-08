# frozen_string_literal: true

require_relative "hip_server/version"

require "zeitwerk"
require "sqlite3"
require "cbor"
require "socket"
require "fileutils"
require "securerandom"


$session_uuid = SecureRandom.uuid

# ROCm HIP compute TCP to Web Frontend gateway server
module HipServer

  def self.session_path(action)
    File.join(__dir__, "../db_files/#{$session_uuid}/#{action}/")
  end

  def self.error_session_path(action)
     File.join(__dir__, "../db_files/errors/#{$session_uuid}/#{action}/")
  end

  class Error < StandardError; end
  # Your code goes here...
end

loader = Zeitwerk::Loader.new

loader.enable_reloading # you need to opt-in before setup
loader.push_dir(__dir__.to_s)

loader.setup

loader.reload

$db = HipServer::Db.new
$db.init